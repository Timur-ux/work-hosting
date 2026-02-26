"use client";

import Input from "@/_components/Input";
import AddWork from "@/_feature/admin/addWork";
import { IsValidResponse, RequestError } from "@/_feature/client";
import { logout } from "@/_reducers/profile";
import { buttonClass } from "@/_styles/globals";
import { useAppDispatch } from "@/hooks";
import Form from "next/form";
import { useState } from "react";

const WorkAddComp = () => {
  const [error, setError] = useState<RequestError | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const dispatch = useAppDispatch();
  const formAction = async (form: FormData) => {
    const workType = form.get("work-type");
    const workNumber = Number(form.get("work-number")?.toString());
    const theme = form.get("theme")?.toString() || "";
    if (workType == null) {
      alert("work type is null!");
      return;
    }
    if (workNumber == 0) {
      alert("work number is null!");
      return;
    }

    const token = localStorage.getItem("bearer-token");
    if (token == null) {
      alert("Token expired");
      dispatch(logout({}));
      return;
    }

    const response = await AddWork(token, {
      "work-type": workType.toString(),
      "old-work-number": workNumber,
      theme: theme,
    });
    if (!IsValidResponse(response)) {
      setError(response.error);
      return;
    } else setSuccess("OK");
  };

  const inputClass = "bg-white rounded w-full md:px-5 md:py-2 text-black";
  return (
    <div className="text-sm md:text-xl">
      {error !== null && (
        <div className="text-red-500 w-full flex justify-center">
          Error: {error.message}
        </div>
      )}
      {success && (
        <div className="text-green-500 text-xl w-full flex justify-center">
          {success}
        </div>
      )}
      <Form className="items-center" action={formAction}>
        <div className="grid gap-5 grid-cols-3 grid-rows-3 place-items-center items-baseline">
          <div>Тип работы</div>
          <div></div>
          <select name="work-type">
            <option>LR</option>
            <option>KP</option>
          </select>
          <div>Номер работы по старому списку</div>
          <div></div>
          <input
            className={inputClass}
            name="work-number"
            type="number"
            min={1}
          />
          <div>Тема работы</div>
          <div></div>
          <input className={inputClass} type="text" name="theme" />
          <div></div>
          <button type="submit" className={buttonClass}>
            Отправить
          </button>
          <div> </div>
        </div>
      </Form>
    </div>
  );
};

export default WorkAddComp;
