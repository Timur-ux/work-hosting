"use server";
import { DoRequest, Response } from "../client";
export type WorkData = {
  work_type: "LR" | "KP";
  old_work_number: number;
};

const SendWork: (
  bearer_token: string,
  work: WorkData,
) => Promise<Response<string>> = async (
  bearer_token: string,
  work: WorkData,
) => {
  const response = await DoRequest(
    "POST",
    "/send",
    bearer_token,
    {
      "work-type": work.work_type,
      "old-work-number": work.old_work_number,
    },
    (data) => data,
  );
  return response;
};

export default SendWork;
