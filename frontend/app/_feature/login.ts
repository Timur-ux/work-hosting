"use server";
import { AxiosError } from "axios";
import client, { RequestError, Response } from "./client";

export type BearerToken = string;
const LoginAndGetToken: (
  username: string,
  password: string,
) => Promise<Response<BearerToken>> = async (
  username: string,
  password: string,
) => {
  try {
    const loginResponse = await client.post("/api/login", {
      username: username,
      password: password,
    });
    if (Math.floor(loginResponse.status / 100) != 2) {
      return {
        payload: null,
				uri: "/api/login",
        error: {
          status: loginResponse.status,
          message: loginResponse.data,
        },
      } as Response<BearerToken>;
    }

    const token = loginResponse.data["bearer-token"];
    return {
			uri: "/api/login",
      payload: token as BearerToken,
      error: null,
    } as Response<BearerToken>;
  } catch (e) {
    const error = e as AxiosError;
    return {
      payload: null,
			uri: "/api/login",
      error: error.response?.data,
    } as Response<BearerToken>;
  }
};

export default LoginAndGetToken;
