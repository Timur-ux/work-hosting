"use server";
import { DoRequest, ResponseWrapper } from "@/_feature/client";

export type AddWorkData = {
	"work-type": string;
	"old-work-number" : number;
	"theme": string;
};

const AddWork: (bearer_token: string, workData: AddWorkData) => Promise<ResponseWrapper<string>> = async (bearer_token, workData) => {
  return await DoRequest("POST", "/admin/work", bearer_token, workData, (d) => d);
};

export default AddWork;
