"use server";
import { DoRequest, getNginxUrl, ResponseWrapper } from "../client";
type WorkData = {
  work_type: "LR" | "KP";
  old_work_number: number;
};

const GetLog: (
  username: string,
  work: WorkData,
) => Promise<ResponseWrapper<string>> = async (
  username: string,
  work: WorkData,
) => {
  const uri = ``;
  try {
    const response = DoRequest(
      "GET",
      `${getNginxUrl()}/logs/${work.work_type}${work.old_work_number}-${username}.log`,
      null,
      null,
      (d) => d,
			true
    );
    return await response;
  } catch (e) {
    return {
      error: e,
      uri: uri,
      payload: null,
    } as ResponseWrapper<string>;
  }
};

export default GetLog;
