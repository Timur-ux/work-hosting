"use client";
import { IsValidResponse, RequestError } from "@/_feature/client";
import GetStudentMarks, { StudentMark } from "@/_feature/student/marks";
import { SelectIsLoggedIn } from "@/_reducers/profile";
import { buttonClass } from "@/_styles/globals";
import { useAppSelector } from "@/hooks";
import Link from "next/link";
import { useEffect, useState } from "react";

const Mark = ({ work_type, old_work_number, status, mark }: StudentMark) => {
  const link = status == "null" ? "/student/send" : "/student/logs";
  return (
    <Link
			className="mx-5"
      href={{
        pathname: link,
        query: {
          work_type: work_type,
          old_work_number: old_work_number,
        },
      }}
    >
      <div className={buttonClass}>
        <div>
          {work_type} {old_work_number}
        </div>
        <div>Оценка: {mark}</div>
        <div>Статус: {status}</div>
      </div>
    </Link>
  );
};

const StudentMarks = () => {
  const loggedIn = useAppSelector(SelectIsLoggedIn);
  const [marks, setMarks] = useState<StudentMark[]>([]);
  const [error, setError] = useState<RequestError | null>(null);

  const getMarks = async () => {
    if (!loggedIn) return;
    const token = localStorage.getItem("bearer-token") as string;
    const marks = await GetStudentMarks(token);
    if (!IsValidResponse(marks)) {
      console.log("Error: ", marks.error);
      setError(marks.error);
      return;
    }
    console.log("Fetched marks: ", marks.payload);
    setMarks(marks.payload as StudentMark[]);
  };

  useEffect(() => {
    getMarks();
  }, [loggedIn]);

  if (!loggedIn) return <div>Вы не вошли в профиль, идите отсюда!</div>;

  const markElements = marks.map((value, index) => (
    <Mark key={index} {...value} />
  ));
  return (
    <div>
      {error != null && (
        <div className="text-red-500">Error: {error.message}</div>
      )}
      <div className="flex">{markElements}</div>
    </div>
  );
};

export default StudentMarks;
