"use client";
import { IsValidResponse, RequestError } from "@/_feature/client";
import GetLog from "@/_feature/student/log";
import GetStudentMarks, { StudentMark } from "@/_feature/student/marks";
import { SelectIsLoggedIn, SelectProfile } from "@/_reducers/profile";
import { buttonClass } from "@/_styles/globals";
import { useAppSelector } from "@/hooks";
import { useEffect, useState } from "react";

type ChoosenWork = {
  work_type: string;
  old_work_number: number;
};

const LogLine = ({ line }: { line: string }) => {
  const status =
    (line.startsWith("[OK]") && "Ok") ||
    (line.startsWith("[ERROR]") && "Error");
  const statusClass =
    status == "Ok" ? "text-green-500" : status == "Error" ? "text-red-900" : "";

  return <div className={statusClass}>{line}</div>;
};

const Logs = () => {
  const loggedIn = useAppSelector(SelectIsLoggedIn);
  const profile = useAppSelector(SelectProfile);
  const [marks, setMarks] = useState<StudentMark[]>([]);
  const [error, setError] = useState<RequestError | null>(null);
  const [log, setLog] = useState<string | null>(null);
  const [choosenWork, setChoosenWork] = useState<ChoosenWork | null>(null);
  const getMarks = async () => {
    console.log(loggedIn);
    if (!loggedIn) return;
    const token = localStorage.getItem("bearer-token") || "";
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
        <div>Выберите работу для получения отчета:</div>
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

  const checkLog = async (work: ChoosenWork) => {
    setError(null);
    setLog(null);
    if (!loggedIn) return;
    const response = await GetLog(profile?.username as string, {
      work_type: work.work_type as "LR" | "KP",
      old_work_number: work.old_work_number,
    });
    if (!IsValidResponse(response)) {
      setError(response.error);
      return;
    }
    setLog(response.payload);
  };

  const lines = (log != null && log.split("\n")) || [];

  return (
    <div>
      {error != null && (
        <div className="text-red-500">Error: {error.message}</div>
      )}
      <div className="w-full flex justify-center my-5">
        Выбранная работа: {choosenWork.work_type} {choosenWork.old_work_number}
      </div>
      <div className="flex justify-center">
        <button
          className={`${buttonClass} mx-5`}
          onClick={async () => {
            await checkLog(choosenWork);
          }}
        >
          Проверить логи проверки
        </button>
        <button
          className={`${buttonClass} mx-5`}
          onClick={() => setChoosenWork(null)}
        >
          Вернуться к списку работ
        </button>
      </div>
      {log != null && (
        <div className="display-newlines bg-gray-500 p-3 rounded mt-10">
          {lines.map((value, index) => {
            return (
              <LogLine line={value} key={index} />
            );
          })}
        </div>
      )}
    </div>
  );
};

export default Logs;
