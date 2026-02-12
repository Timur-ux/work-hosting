"use client";
import { IsValidResponse, RequestError } from "@/_feature/client";
import GetStudentMarks, { StudentMark } from "@/_feature/student/marks";
import { SelectIsLoggedIn } from "@/_reducers/profile";
import { buttonClass } from "@/_styles/globals";
import { useAppSelector } from "@/hooks";
import { Marko_One } from "next/font/google";
import Link from "next/link";
import { useEffect, useState } from "react";

const Mark = ({ work_type, old_work_number, status, mark }: StudentMark) => {
  const link = status == "null" ? "/student/send" : "/student/logs";
  return (
    <Link href={{
			pathname: link,
			query: {
				work_type: work_type,
				old_work_number: old_work_number
			}
		}}>
      <div className={buttonClass}>
        <div>
          {work_type} {old_work_number}
        </div>
				<div>
				Оценка: {mark}
				</div>
				<div>
				Статус: {status}
				</div>
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
      setError(marks.error);
      return;
    }
    setMarks(marks.payload as StudentMark[]);
  };

  useEffect(() => {
    getMarks();
    setMarks([
      {
        work_type: "LR",
        old_work_number: 11,
        mark: "2",
        status: "rejected",
      },
    ]);
  }, []);

  if (!loggedIn) return <div>Вы не вошли в профиль, идите отсюда!</div>;

  const markElements = marks.map((value, index) => <Mark key={index} {...value}/>);

  return <div>{markElements}</div>;
};

export default StudentMarks;
