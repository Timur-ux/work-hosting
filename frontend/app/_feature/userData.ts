import { ProfileData, StudentData } from "@/_reducers/profile";
import client, {
  CastResponse,
  IsValidResponse,
  RequestError,
  Response,
} from "./client";
import GetUserRole from "./role";
import { AxiosError } from "axios";

const GetStudentData: (
  bearer_token: string,
) => Promise<Response<StudentData>> = async (bearer_token: string) => {
  try {
    const response = await client.get("/api/student/profile", {
      headers: { Authorization: `Bearer ${bearer_token}` },
    });

    if (Math.floor(response.status / 100) != 2) {
      return {
        uri: "/api/student/profile",
        payload: null,
        error: {
          status: response.status,
          message: response.statusText,
        },
      } as Response<StudentData>;
    }

    const studentData = response.data;
    return {
      uri: "/api/student/profile",
      error: null,
      payload: {
        first_name: studentData["first-name"],
        last_name: studentData["last-name"],
        father_name: studentData["father-name"],
        email: studentData["email"],
        group_number: Number(studentData["group-number"]),
        in_group_order: Number(studentData["in-group-order"]),
      },
    };
  } catch (e) {
    const error = e as AxiosError;
    return {
      uri: "/api/student/profile",
      payload: null,
			error: error.response?.data
    } as Response<StudentData>;
  }
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
