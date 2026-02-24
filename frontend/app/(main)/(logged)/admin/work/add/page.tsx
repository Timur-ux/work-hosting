"use client";

import Input from "@/_components/Input";
import { RequestError } from "@/_feature/client";
import { buttonClass } from "@/_styles/globals";
import Form from "next/form";
import { useState } from "react";

const WorkAddComp = () => {
  const [error, setError] = useState<RequestError | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const formAction = async (form: FormData) => {};
  const inputClass = "bg-white rounded w-full md:px-5 md:py-2 text-black";
  return (
    <div className="text-sm md:text-xl">
      {error !== null && <div className="text-red-500">error.message</div>}
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
          <input className={inputClass} type="number" min={1} />
          <div>Тема работы</div>
          <div></div>
          <input className={inputClass} type="text" />
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
