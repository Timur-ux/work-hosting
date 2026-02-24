"use server";
import { DoRequest, ResponseWrapper } from "./client";

export type QueuedWork = {
  gvName: string;
  work_type: "LR" | "KP";
  old_work_number: number;
};

const GetQueuedWorks: () => Promise<ResponseWrapper<QueuedWork[]>> = async () => {
  return await DoRequest("GET", "/queue", null, null, (d) => {
		const data = JSON.parse(d);
		console.log(data)
    return data.map((data: any) => {
      return {
        gvName: data["gv-name"],
        work_type: data["work-type"],
        old_work_number: data["old-work-number"],
      } as QueuedWork;
    });
  });
};

export default GetQueuedWorks;
