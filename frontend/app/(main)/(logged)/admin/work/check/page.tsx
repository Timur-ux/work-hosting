"use client";
import PopWorkRequest from "@/_feature/admin/popWork";
import { IsValidResponse, RequestError } from "@/_feature/client";
import { logout } from "@/_reducers/profile";
import { buttonClass } from "@/_styles/globals";
import { useAppDispatch } from "@/hooks";
import Link from "next/link";
import { useSearchParams } from "next/navigation";
import React, { useState } from "react";

const CheckWork = () => {
  const searchParams = useSearchParams();
  const [error, setError] = useState<RequestError | null>(null);
  const [success, setSuccess] = useState<string | null>(null);
  const dispatch = useAppDispatch();

  const workType = searchParams.get("work-type")?.toString();
  const workNumber = Number(searchParams.get("work-number")?.toString());
  const gvName = searchParams.get("gv-name")?.toString();

  if (workType == null) {
    alert("Work type is null!");
    return;
  }
  if (workNumber == 0) {
    alert("Work number is 0!");
    return;
  }
  if (gvName == null) {
    alert("Gitverse name is null!");
    return;
  }

  const PopWork = (action: "accept" | "reject" | "requeue") => async () => {
    const token = localStorage.getItem("bearer-token");
    if (token == null) {
      alert("Token expired!");
      dispatch(logout({}));
      return;
    }

    const response = await PopWorkRequest(token, action, {
      "work-type": workType.toString(),
      "old-work-number": workNumber,
      "gv-name": gvName,
    });
    if (!IsValidResponse(response)) {
      setError(response.error);
      return;
    } else setSuccess("OK");
  };

  return (
    <div>
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
      <div className="grid gap-5 grid-cols-2 my-10">
        <div>Ссылка на репозиторий:</div>
        <div>
          <Link href={`https://gitverse.ru/mai_labs_2025_2026/${workType}${workNumber}-${gvName}/content/solution/solution`}>
						{`https://gitverse.ru/mai_labs_2025_2026/${workType}${workNumber}-${gvName}/content/solution/solution`}
          </Link>
        </div>
        <div>Ссылка на запрос на слияние:</div>
        <div>
          <Link href={`https://gitverse.ru/mai_labs_2025_2026/${workType}${workNumber}-${gvName}/pulls/1`}>
						{`https://gitverse.ru/mai_labs_2025_2026/${workType}${workNumber}-${gvName}/pulls/1`}
          </Link>
        </div>
      </div>
      <div className="flex w-full justify-center gap-10">
        <button
          className={`${buttonClass} bg-green-500`}
          onClick={() => {
            PopWork("accept")();
          }}
        >
          Принять
        </button>
        <button
          className={`${buttonClass}`}
          onClick={() => {
            PopWork("requeue")();
          }}
        >
          Переотправить в очередь
        </button>
        <button
          className={`${buttonClass} bg-red-500`}
          onClick={() => {
            PopWork("reject")();
          }}
        >
          Отклонить
        </button>
      </div>
    </div>
  );
};

export default CheckWork;
