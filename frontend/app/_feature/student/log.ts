"use server";
import { Response } from "../client";
type WorkData = {
  work_type: "LR" | "KP";
  old_work_number: number;
};

const GetLog: (
  username: string,
  work: WorkData,
) => Promise<Response<string>> = async (username: string, work: WorkData) => {
  const uri = `http://localhost/logs/${work.work_type}${work.old_work_number}-${username}.log`;
  try {
    const response = await fetch(uri);
    return {
      error: null,
      uri: uri,
      payload: await response.text(),
    };
  } catch (e) {
    return {
      error: e,
      uri: uri,
      payload: null,
    } as Response<string>;
  }
};

export default GetLog;
