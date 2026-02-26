"use server";

import GetQueuedWorks, { QueuedWork } from "@/_feature/checkingQueue";
import { IsValidResponse, RequestError } from "@/_feature/client";

const getQueuedWorksWrapped = async () => {
  const works: QueuedWork[] = [];
  let error: RequestError | null = null;

  const getWorks = async () => {
    works.push(...(response.payload as QueuedWork[]));
  };

  await getWorks();
  if (error != null) return error;

  return works;
};

export default getQueuedWorksWrapped;
