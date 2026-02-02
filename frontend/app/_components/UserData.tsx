"use client";
import React from "react";
import { useAppDispatch, useAppSelector } from "@/hooks";
import { workType, workNumber, gvName, selectUserData, UserDataState } from "@/_reducers/userData";

const UserData = () => {
  const dispatch = useAppDispatch();
  const itemClass = "mx-5 my-5 text-center";
  const fieldClass = "mx-5 bg-gray-500 p-2 rounded";
	const udata = useAppSelector(selectUserData);
  return (
    <div className="flex w-full justify-center my-5">
      <div className="grid h-56 grid-cols-1 grid-rows-3 lg:grid-cols-3 lg:grid-rows-1 gap-5 content-center items-baseline-last">
        <div className="content-center">
          <div className={itemClass}>Тип работы</div>
          <div className={itemClass}>
            <select
              className={fieldClass}
              defaultValue={"Unset"}
              onChange={(e) => dispatch(workType(e.target.value as ("LR" | "KP" | "Unset")))}
            >
              <option>LR</option>
              <option>KP</option>
              <option disabled>Unset</option>
            </select>
          </div>
        </div>
        <div className="content-center">
          <div className={itemClass}>
            Номер работы по старому списку
            <br />
            (соответствует номерам работ в таблице с оценками)
          </div>
          <div className={itemClass}>
            <input
              type="number"
              className={fieldClass}
              onChange={(e) => dispatch(workNumber(Number(e.target.value.toString())))}
							defaultValue={-1}
            />
          </div>
        </div>
        <div className="content-center">
          <div className={itemClass}>Имя пользователя</div>
          <div className={itemClass}>
            <input
              className={fieldClass}
              placeholder="Gitverse ник без знака @"
              onChange={(e) => dispatch(gvName(e.target.value))}
							defaultValue={""}
            />
          </div>
        </div>
      </div>
    </div>
  );
};

export default UserData;
