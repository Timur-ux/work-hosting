"use server";
import { AxiosError } from "axios";
import client, { Response } from "./client";

export type UserRole = string;
const GetUserRole: (
  bearer_token: string,
) => Promise<Response<UserRole>> = async (bearer_token: string) => {
  try {
    const response = await client.get("/user/role", {
      headers: { Authorization: `Bearer ${bearer_token}` },
    });
    if (Math.floor(response.status / 100) != 2) {
      return {
				uri: "/user/role",
        payload: null,
        error: {
          status: response.status,
          message: response.data,
        },
      };
    }

    const userRole = response.data;
    return {
			uri: "/user/role",
      payload: userRole,
      error: null,
    };
  } catch (e) {
    const error = e as AxiosError;
    return {
      payload: null,
			uri: "/user/role",
      error: {
        status: error.status,
        message: error.message,
      },
    };
  }
};

export default GetUserRole;
