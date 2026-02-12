import axios, { AxiosError, AxiosResponse } from "axios";

const client = axios.create({
  baseURL: "http://localhost:80/api",
});

client.interceptors.request.use(
  (value) => {
    console.log("Request url: ", value.url);
    return value;
  },
  (error) => error,
);

client.interceptors.response.use(
  (value) => value,
  (error) => {
    console.log("INTERCEPT ERROR: ", error);
    if ("response" in error && "code" in error["response"])
      error["response"]["status"] = error["response"]["code"];
    return error;
  },
);

export type RequestError = {
  status: number;
  message: string;
};

export type Response<T> = {
  uri: string | null;
  payload: T | null;
  error: RequestError | null;
};

const Get = async (uri: string, token: string | null) => {
  if (token == null) return await client.get(uri);

  return await client.get(uri, {
    headers: { Authorization: `Bearer ${token}` },
  });
};

const Post = async (uri: string, token: string | null, data: any) => {
  if (token == null) return await client.post(uri, data);

  return await client.post(uri, data, {
    headers: { Authorization: `Bearer ${token}` },
  });
}

const Delete = async (uri: string, token: string | null) => {
  if (token == null) return await client.delete(uri);

  return await client.delete(uri, {
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
		let response: AxiosResponse<any, any, {}> | null = null;
    if(method == "GET")
		 response =	await Get(uri, token);
		else if(method == "POST")
			response = await Post(uri, token, data);
		else if(method == "DELETE")
			response = await Delete(uri, token);
		else
			throw "Undefined Method!";

    if (Math.floor(response.status / 100) != 2) {
      return {
        uri: uri,
        payload: null,
        error: {
          status: response.status,
          message: response.data,
        },
      } as Response<T>;
    }

    const result = mapper(response.data);
    return {
      uri: uri,
      payload: result,
      error: null,
    } as Response<T>;
  } catch (e) {
    const error = e as AxiosError;
    return {
      payload: null,
      uri: uri,
      error: error.response?.data,
    } as Response<T>;
  }
};


export const IsValidResponse = <T extends unknown>(response: Response<T>) =>
  response.payload !== null && response.error == null;

export const CastResponse = <From extends unknown, To extends unknown>(
  response: Response<From>,
) => {
  return {
    uri: response.uri,
    payload: null,
    error: response.error,
  } as Response<To>;
};

export default client;
