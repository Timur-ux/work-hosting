"use client";
import React, { useState } from "react";
import UserData from "@/_components/UserData";
import {
  initialUserDataState,
  selectUserData,
  UserDataState,
} from "@/_reducers/userData";
import { useAppSelector } from "@/hooks";

const Check = () => {
  const udata = useAppSelector(selectUserData);
  const [error, setError] = useState<string | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const check = async (udata: UserDataState) => {
    try {
      const result = await fetch(
        `http://localhost/logs/${udata.workType}${udata.workNumber}-${udata.gvName}.log`,
      );
      console.log(result.status, result.text);
      if (!result.ok) setError(await result.text());
      else {
        const text = await result.text();
        console.log(text);
        setSuccess(text);
      }
    } catch (error) {
      setError((error as Error).message);
    }
  };

  return (
    <div className="mb-10 bg-black" onLoad={() => {setError(null); setSuccess(null);}}>
      <UserData />
      <div className="flex w-full justify-center">
        <button
          className="bg-blue-500 rounded-xl p-3 hover:bg-blue-300"
          onClick={async () => await check(udata)}
        >
          Запросить отчет
        </button>
      </div>
      {error != null && <p className="text-red-500">Сетевая ошибка: {error}</p>}
      {success != null && <div className="display-newlines">{success}</div>}
    </div>
  );
};

export default Check;
