"use server";
import { AxiosError } from "axios";
import client, { DoRequest, Response } from "./client";

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
	const response = await DoRequest("POST", "/register", null, {
      "gv-name": username,
			"first-name": first_name,
			"last-name" : last_name,
			"father-name" : father_name,
			"email" : email,
      "password": password
    }, (data) => data);
	return response;
};

export default Register;

