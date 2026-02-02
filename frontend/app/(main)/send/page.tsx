"use client";
import UserData from "@/_components/UserData";
import { useAppSelector } from "@/hooks";
import {
  initialUserDataState,
  selectUserData,
  UserDataState,
} from "@/_reducers/userData";
import { useState } from "react";
import { redirect } from "next/navigation";

const Send = () => {
  const udata = useAppSelector(selectUserData);
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const send = async (udata: UserDataState) => {
    try {
      const result = await fetch("http://localhost/api/send", {
        method: "POST",
        body: JSON.stringify(udata),
      });
			console.log(result.status, result.text)
      if (!result.ok) setError(await result.text());
			else setSuccess(await result.text());
    } catch (error) {
      setError((error as Error).message);
    }
  };
  return (
    <div>
      <UserData />
      <div className="flex w-full justify-center">
        <button
          className="bg-blue-500 rounded-xl p-3 hover:bg-blue-300"
          onClick={async () => send(udata)}
        >
          Отправить работу
        </button>
      </div>
      {error != null && <p className="text-red-500">Ошибка: {error}</p>}
      {success != null && <p className="text-green-500">Успех: {success} <br/>Теперь вы можете посмотреть отчет о проверке</p>}
    </div>
  );
};

export default Send;
