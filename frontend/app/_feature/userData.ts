"use server";
import { ProfileData, StudentData } from "@/_reducers/profile";
import client, {
  CastResponse,
  DoRequest,
  IsValidResponse,
  Response,
} from "./client";
import GetUserRole from "./role";
import { AxiosError } from "axios";

const GetStudentData: (
  bearer_token: string,
) => Promise<Response<StudentData>> = async (bearer_token: string) => {
	const response = await DoRequest("GET", "/student/profile", bearer_token, null, (data) => data);
	return response;
};

const GetUserData: (
  bearer_token: string,
  username: string,
) => Promise<Response<ProfileData>> = async (
  bearer_token: string,
  username: string,
) => {
  const response = await GetUserRole(bearer_token);
  if (!IsValidResponse(response)) return CastResponse(response);
  const userRole = response.payload;
  if (userRole == "admin") {
    return {
      uri: null,
      payload: {
        username: username,
        role: "admin",
        data: null,
      } as ProfileData,
      error: null,
    };
  } else if (userRole != "student") {
    return {
      uri: response.uri,
      payload: null,
      error: {
        status: 500,
        message: `Get unknown user role: ${userRole}`,
      },
    };
  }

  const studentDataResponse = await GetStudentData(bearer_token);
  if (!IsValidResponse(studentDataResponse))
    return CastResponse(studentDataResponse);
  const studentData = studentDataResponse.payload;
  return {
    uri: null,
    error: null,
    payload: {
      username: username,
      role: userRole,
      data: studentData,
    },
  };
};

export default GetUserData;
