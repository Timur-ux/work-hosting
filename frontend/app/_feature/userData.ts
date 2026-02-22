"use server";
import { ProfileData, StudentData } from "@/_reducers/profile";
import { CastResponse, DoRequest, IsValidResponse, ResponseWrapper } from "./client";
import GetUserRole from "./role";

const nullStudentData: any = {
  email: "null",
  "first-name": "null",
  "last-name": "null",
  "father-name": "null",
  "group-number": -1,
  "in-group-order": -1,
};

const GetStudentData: (
  bearer_token: string,
) => Promise<ResponseWrapper<StudentData>> = async (bearer_token: string) => {
  const response = await DoRequest(
    "GET",
    "/student/profile",
    bearer_token,
    null,
    (data) => data,
  );
  return response;
};

const GetUserData: (
  bearer_token: string,
  username: string,
) => Promise<ResponseWrapper<ProfileData>> = async (
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
  const raw =
    studentDataResponse.payload != null
      ? studentDataResponse.payload
      : nullStudentData;
  const studentData: StudentData = {
    email: raw["email"],
    first_name: raw["first-name"],
    last_name: raw["last-name"],
    father_name: raw["father-name"],
    group_number: raw["group-number"],
    in_group_order: raw["in-group-order"],
  };
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
