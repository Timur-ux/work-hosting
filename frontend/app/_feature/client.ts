const getBaseUrl = () => {
  return "http://api:8080";
  // return "http://localhost:80/api";
};

const baseUrl = getBaseUrl();

export type RequestError = {
  status: number;
  message: string;
};

export type ResponseWrapper<T> = {
  uri: string | null;
  payload: T | null;
  error: RequestError | null;
};

const Get = async (uri: string, token: string | null) => {
	console.log(baseUrl + uri, process.env.DEV)
  if (token == null)
    return await fetch(baseUrl + uri, {
      cache: "no-store",
    });

  return await fetch(baseUrl + uri, {
    headers: { Authorization: `Bearer ${token}` },
    cache: "no-store",
  });
};

const Post = async (uri: string, token: string | null, data: any) => {
  if (token == null)
    return await fetch(baseUrl + uri, {
      body: data,
    });

  return await fetch(baseUrl + uri, {
    headers: { Authorization: `Bearer ${token}` },
    body: data,
  });
};

const Delete = async (uri: string, token: string | null) => {
  if (token == null) return await fetch(baseUrl + uri);

  return await fetch(uri, {
    headers: { Authorization: `Bearer ${token}` },
  });
};

export const DoRequest = async <T extends unknown>(
  method: "GET" | "POST" | "DELETE",
  uri: string,
  token: string | null,
  data: any,
  mapper: (data: any) => T,
) => {
  try {
    let response: Response | null = null;
    if (method == "GET") response = await Get(uri, token);
    else if (method == "POST") response = await Post(uri, token, data);
    else if (method == "DELETE") response = await Delete(uri, token);
    else throw "Undefined Method!";

    if (Math.floor(response.status / 100) != 2) {
      console.log("RESPONSE STATUS NOT 2xx: ", response);
      return {
        uri: uri,
        payload: null,
        error: {
          status: response.status,
          message: await response.text(),
        },
      } as ResponseWrapper<T>;
    }

    const result = mapper(await response.json());
    return {
      uri: uri,
      payload: result,
      error: null,
    } as ResponseWrapper<T>;
  } catch (e) {
    const error = e as RequestError;
    console.log("CATCHED ERROR: ", error);
    return {
      payload: null,
      uri: uri,
      error: error,
    } as ResponseWrapper<T>;
  }
};

export const IsValidResponse = <T extends unknown>(
  response: ResponseWrapper<T>,
) => response.payload !== null && response.error == null;

export const CastResponse = <From extends unknown, To extends unknown>(
  response: ResponseWrapper<From>,
) => {
  return {
    uri: response.uri,
    payload: null,
    error: response.error,
  } as ResponseWrapper<To>;
};
