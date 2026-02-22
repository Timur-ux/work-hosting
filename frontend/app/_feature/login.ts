"use server";
import { AxiosError } from "axios";
import  { DoRequest, ResponseWrapper } from "./client";

export type BearerToken = string;
const LoginAndGetToken: (
  username: string,
  password: string,
) => Promise<ResponseWrapper<BearerToken>> = async (
  username: string,
  password: string,
) => {
	const token = await DoRequest<BearerToken>("POST", "/login", null, {
      username: username,
      password: password,
    }, (data) => data["bearer-token"]);

	return token;
};

export default LoginAndGetToken;
