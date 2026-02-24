"use client";
import { IsValidResponse, RequestError } from "@/_feature/client";
import GetStudentMarks, { StudentMark } from "@/_feature/student/marks";
import { SelectIsLoggedIn } from "@/_reducers/profile";
import { buttonClass } from "@/_styles/globals";
import { useAppSelector } from "@/hooks";
import { useEffect, useState } from "react";
import SendWorkFunc from "@/_feature/student/send";

type ChoosenWork = {
  work_type: string;
  old_work_number: number;
};

const SendWork = () => {
  const loggedIn = useAppSelector(SelectIsLoggedIn);
  const [marks, setMarks] = useState<StudentMark[]>([]);
  const [error, setError] = useState<RequestError | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const [choosenWork, setChoosenWork] = useState<ChoosenWork | null>(null);
  const getMarks = async () => {
    if (!loggedIn) return;
    const token = localStorage.getItem("bearer-token") as string;
    const marks = await GetStudentMarks(token);
    if (!IsValidResponse(marks)) {
      setError(marks.error);
      return;
    }
    setMarks(marks.payload as StudentMark[]);
  };

  useEffect(() => {
    getMarks();
  }, [loggedIn]);

  if (!loggedIn) return <div>Вы не вошли в профиль, идите отсюда!</div>;
  if (choosenWork == null) {
    return (
      <div>
        <div>Выберите работу для отправки:</div>
        <div className="flex justify-center my-5">
          {marks.map((value, index) => (
            <button
              key={index}
              className={`${buttonClass} mx-5`}
              onClick={() =>
                setChoosenWork({
                  work_type: value.work_type,
                  old_work_number: value.old_work_number,
                })
              }
            >
              {value.work_type} {value.old_work_number}
            </button>
          ))}
        </div>
      </div>
    );
  }

  const sendWork = async (work: ChoosenWork) => {
    setError(null);
    setSuccess(null);
    const token = localStorage.getItem("bearer-token");
    const response = await SendWorkFunc(token as string, {
      work_type: work.work_type as "LR" | "KP",
      old_work_number: work.old_work_number,
    });

    if (!IsValidResponse(response)) setError(response.error);
    else setSuccess(response.payload);
  };

	console.log("Send: ", error, success);
  return (
    <div>
      {error != null && (
        <div className="text-red-500">Error: {error.message}</div>
      )}
      {success != null && <div className="text-green-500">OK: {success}</div>}

      <div className="w-full flex justify-center my-5">
        Выбранная работа: {choosenWork.work_type} {choosenWork.old_work_number}
      </div>
      <div className="flex justify-center">
        <button
          className={`${buttonClass} mx-5`}
          onClick={async () => await sendWork(choosenWork)}
        >
          Отправить работу
        </button>
        <button
          className={`${buttonClass} mx-5`}
          onClick={() => setChoosenWork(null)}
        >
          Вернуться к списку работ
        </button>
      </div>
    </div>
  );
};

export default SendWork;
