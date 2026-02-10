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
    const loginResponse = await client.post("/login", {
      username: username,
      password: password,
    });
    if (Math.floor(loginResponse.status / 100) != 2) {
      return {
        payload: null,
				uri: "/login",
        error: {
          status: loginResponse.status,
          message: loginResponse.data,
        },
      };
    }

    const token = loginResponse.data["bearer-token"];
    return {
			uri: "/login",
      payload: token as BearerToken,
      error: null,
    };
  } catch (e) {
    const error = e as AxiosError;
    return {
      payload: null,
			uri: "/login",
      error: {
        status: error.status,
        message: error.message,
      },
    };
  }
};

export default LoginAndGetToken;
