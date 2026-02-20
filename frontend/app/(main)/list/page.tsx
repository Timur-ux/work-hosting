"use server";
import GetQueuedWorks, { QueuedWork } from "@/_feature/checkingQueue";
import { IsValidResponse, RequestError } from "@/_feature/client";

const dummyError: RequestError = {
  status: -100,
  message: "It's dummy error, you don't should see it",
};

export default async () => {
  const works: QueuedWork[] = [];
  let isError: boolean = false;
  let error: RequestError = dummyError;

  const getWorks = async () => {
    const response = await GetQueuedWorks();
    if (!IsValidResponse(response)) {
      console.log("Error: ", response.error);
      isError = true;
      error = (response.error != null && response.error) || dummyError;
      return;
    }
    works.push(...(response.payload as QueuedWork[]));
  };

  await getWorks();

	const cellClass = "px-5 border-x-2"
  const workElements = works.map((value, index) => (
    <tr key={index}>
      <td className={cellClass}>{value.gvName}</td>
      <td className={cellClass}>{value.work_type}</td>
      <td className={cellClass}>{value.old_work_number}</td>
    </tr>
  ));
  return (
    <div>
      {isError && <div className="text-red-500">Error: {error.message}</div>}
      <table className="table-auto border-2">
        <caption>Очередь на проверку(сверху вниз)</caption>
        <thead>
          <tr>
            <th scope="col" className={cellClass}>Имя пользователя</th>
            <th scope="col" className={cellClass}>Тип работы</th>
            <th scope="col" className={cellClass}>Номер работы</th>
          </tr>
        </thead>
        <tbody className="border-t-2">
				{workElements}</tbody>
      </table>
    </div>
  );
};
