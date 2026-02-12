"use server";
import { AxiosError } from "axios";
import client, { DoRequest, Response } from "./client";

export type UserRole = string;
const GetUserRole: (
  bearer_token: string,
) => Promise<Response<UserRole>> = async (bearer_token: string) => {
  const response = await DoRequest(
    "GET",
    "/user/role",
    bearer_token,
    null,
    (data) => data,
  );
  return response;
};

export default GetUserRole;
