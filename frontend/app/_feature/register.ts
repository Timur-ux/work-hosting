"use server";
import { AxiosError } from "axios";
import client, { Response } from "./client";

export type RegisterResult = "success";
const Register: (
  username: string,
  password: string,
	first_name: string,
	last_name: string,
	father_name: string,
	email: string
) => Promise<Response<RegisterResult>> = async (
  username: string,
  password: string,
	first_name: string,
	last_name: string,
	father_name: string,
	email: string
) => {
  try {
    const response = await client.post("/api/register", {
      "gv-name": username,
			"first-name": first_name,
			"last-name" : last_name,
			"father-name" : father_name,
			"email" : email,
      "password": password
    });
    if (Math.floor(response.status / 100) != 2) {
      return {
        payload: null,
				uri: "/api/register",
        error: {
          status: response.status,
          message: response.statusText + response.data,
        },
      } as Response<RegisterResult>;
    }

    const result = response.data;
    return {
			uri: "/api/register",
      payload: result as RegisterResult,
      error: null,
    } as Response<RegisterResult>;
  } catch (e) {
    const error = e as AxiosError;
    return {
      payload: null,
			uri: "/api/register",
      error: error.response?.data,
		} as Response<RegisterResult>;
  }
};

export default Register;

