"use server";
import { DoRequest, ResponseWrapper } from "@/_feature/client";

export type PopWorkData = {
	"work-type": string;
	"old-work-number" : number;
	"gv-name" : string
};
const PopWorkRequest: (bearer_token: string, action: "accept" | "reject" | "requeue", workData: PopWorkData) => Promise<ResponseWrapper<string>> = async (bearer_token, action, workData) => {
  return await DoRequest("POST", `/admin/queue?action=${action}`, bearer_token, workData, (d) => d);
};

export default PopWorkRequest;

