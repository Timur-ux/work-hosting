'use server';
import client, { RequestError } from "./client";

export type UserRole = string;
const GetUserRole: (bearer_token: string) => Promise<UserRole> = async (bearer_token: string) => {
  try {
    const response = await client.get("/user/role", {
			headers: {"Authorization": `Bearer ${bearer_token}`}
		});
		
		if(Math.floor(response.status / 100) != 2)
			console.warn("Get user role request return not 2xx status code: ", response.status);

    const userRole = response.data;
    return userRole as UserRole;
  } catch (e) {
    const error = e as Error;
    throw JSON.stringify({
      status: 500,
      message: error.message,
    } as RequestError);
  }
};

export default GetUserRole;

