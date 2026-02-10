import axios from "axios";

const client = axios.create({
  baseURL: "http://localhost:80/api",
});

export type RequestError = {
  status: number;
  message: string;
};

export type Response<T> = {
  uri: string | null;
  payload: T | null;
  error: RequestError | null;
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
