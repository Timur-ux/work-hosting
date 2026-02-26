"use server";
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
    }, (data) => JSON.parse(data)["bearer-token"]);

	return token;
};

export default LoginAndGetToken;
