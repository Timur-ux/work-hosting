const getBaseUrl = () => {
  // return "http://api:8080";
  return "http://localhost:80/api";
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
  if (token == null)
    return await fetch(baseUrl + uri, {
      method: "GET",
      cache: "no-store",
    });

  return await fetch(baseUrl + uri, {
    headers: { Authorization: `Bearer ${token}` },
    method: "GET",
    cache: "no-store",
  });
};

const Post = async (uri: string, token: string | null, data: any) => {
  if (token == null)
    return await fetch(baseUrl + uri, {
      method: "POST",
      body: JSON.stringify(data),
    });

  return await fetch(baseUrl + uri, {
    method: "POST",
    headers: { Authorization: `Bearer ${token}` },
    body: JSON.stringify(data),
  });
};

const Delete = async (uri: string, token: string | null) => {
  if (token == null) return await fetch(baseUrl + uri, { method: "DELETE" });

  return await fetch(uri, {
    headers: { Authorization: `Bearer ${token}` },
    method: "DELETE",
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
          message: response.statusText + ": " + await response.text(),
        },
      } as ResponseWrapper<T>;
    }

		const text  = await response.text();
    const result = mapper(text);
    return {
      uri: uri,
      payload: result,
      error: null,
    } as ResponseWrapper<T>;
  } catch (e) {
    const error = e as Error;
    console.log("Uri: ", uri, "Method:", method);
    console.log("CATCHED ERROR: ", error);
    return {
      payload: null,
      uri: uri,
      error: {
				status: error.cause,
				message: error.message
			},
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
