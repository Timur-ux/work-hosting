'use server';
import { ifError } from "assert";
import client, { RequestError } from "./client";

export type BearerToken = string;
const LoginAndGetToken: (
  username: string,
  password: string,
) => Promise<BearerToken> = async (
  username: string,
  password: string,
) => {
  try {
    const loginResponse = await client.post("/login", {
      username: username,
      password: password,
    });
		if(Math.floor(loginResponse.status / 100) != 2)
			console.warn("Login request return not 2xx status code: ", loginResponse.status);
		
    const token = loginResponse.data["bearer-token"];
    return token as BearerToken;
  } catch (e) {
    const error = e as Error;
    throw JSON.stringify({
      status: 500,
      message: error.message,
    } as RequestError);
  }
};

export default LoginAndGetToken;
