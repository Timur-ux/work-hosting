"use server";
import  { DoRequest, ResponseWrapper } from "../client";
export type StudentMark = {
  work_type: "LR" | "KP";
  old_work_number: number;
  mark: "0" | "2" | "3" | "4" | "5";
  status: "null" | "checking" | "accepted" | "rejected";
};

const GetStudentMarks: (
  bearer_token: string,
) => Promise<ResponseWrapper<StudentMark[]>> = async (bearer_token: string) => {
  const response = await DoRequest(
    "GET",
    "/student/marks",
    bearer_token,
    null,
    (d) => {
			const data = JSON.parse(d)
      if (!Array.isArray(data))
        console.log("Undefined result type! Expected array. Data: ", data);
      const result: StudentMark[] = data.map((value: any) => {
        return {
          work_type: value["work-type"],
          old_work_number: value["old-work-number"],
          mark: value["mark"],
          status: value["status"],
        };
      });
      return result;
    },
  );
  return response;
};

export default GetStudentMarks;
