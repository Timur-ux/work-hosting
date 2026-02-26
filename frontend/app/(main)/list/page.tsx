"use client";
import GetQueuedWorks, { QueuedWork } from "@/_feature/checkingQueue";
import { IsValidResponse, RequestError } from "@/_feature/client";
import { SelectProfile } from "@/_reducers/profile";
import { useAppSelector } from "@/hooks";
import Link from "next/link";
import { useRouter } from "next/navigation";
import { useEffect, useState } from "react";

export default () => {
  const [error, setError] = useState<RequestError | null>(null);
  const [works, setWorks] = useState<QueuedWork[]>([]);
  const profile = useAppSelector(SelectProfile);
  const router = useRouter();

  const fetchMarks = async () => {
    const response = await GetQueuedWorks();
    if (!IsValidResponse(response)) {
      console.log("Error: ", response.error);
      setError(response.error);
      return;
    }
    setWorks(response.payload as QueuedWork[]);
  };

  useEffect(() => {
    if (typeof window == "undefined" || works.length > 0) return;
    fetchMarks();
  }, []);

  const cellClass = "px-5 border-2";
  const worksElements = works.map((value, i) => (
      <tr
        key={i}
        onClick={() => {
          if (profile == null || profile.role != "admin") return;
          router.push(
            `/admin/work/check?work-type=${value.work_type}&work-number=${value.old_work_number}&gv-name=${value.gvName}`,
          );
        }}
      >
        <td className={cellClass}>{value.gvName}</td>
        <td className={cellClass}>{value.work_type}</td>
        <td className={cellClass}>{value.old_work_number}</td>
      </tr>
	));
  return (
    <div>
      {error != null && (
        <div className="text-red-500">Error: {error.message}</div>
      )}
      <table className="table-auto border-2">
        <caption>Очередь на проверку(сверху вниз)</caption>
        <thead>
          <tr>
            <th scope="col" className={cellClass}>
              Имя пользователя
            </th>
            <th scope="col" className={cellClass}>
              Тип работы
            </th>
            <th scope="col" className={cellClass}>
              Номер работы
            </th>
          </tr>
        </thead>
        <tbody>{worksElements}</tbody>
      </table>
    </div>
  );
};
