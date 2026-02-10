"use client";
import React, { useState } from "react";
import Form from "next/form";
import { buttonClass } from "@/_styles/globals";
import LoginAndGetToken, { BearerToken } from "@/_feature/login";
import client, { RequestError } from "@/_feature/client";
import NetworkError from "@/_components/NetworkError";
import GetUserRole from "@/_feature/role";
import { useAppDispatch } from "@/hooks";
import { login } from "@/_reducers/profile";

const Login = () => {
  const [error, setError] = useState<RequestError | null>(null);
  const dispatch = useAppDispatch();
  async function ProcessLogin(formData: FormData) {
		setError(null);
    const username = formData.get("username") as string;
    const password = formData.get("password") as string;
    let bearerToken = "";
    try {
      bearerToken = await LoginAndGetToken(username, password);
    } catch (e) {
      const error = e as Error;
      setError(JSON.parse(error.message));
      return;
    }
    localStorage.setItem("bearer-token", bearerToken);
    let userRole = null;
    try {
      userRole = await GetUserRole(bearerToken);
    } catch (e) {
      const error = e as Error;
      setError(JSON.parse(error.message));
      return;
    }
    if (userRole === null) {
      setError({ status: 500, message: "Can't get user role" });
      return;
    } else if (userRole == "admin") {
      dispatch(
        login({
          profile: {
            username: username,
            role: "admin",
            data: null,
          },
        }),
      );
    } else if (userRole != "student") {
      setError({
        status: 500,
        message: ` Server return undefined user role: ${userRole}`,
      });
    }

    let studentData = null;
    try {
    } catch (e) {
      const error = e as Error;
      setError(JSON.parse(error.message));
      return;
    }
  }

  const inputClass = "bg-white rounded w-full my-2 md:px-5 md:py-2 text-black";
  return (
    <div className="text-sm md:text-xl">
      {error !== null && <NetworkError error={error} />}
      <Form className="items-center" action={ProcessLogin}>
        <div className="grid grid-cols-3 grid-rows-3 place-items-center items-baseline">
          <div>Логин(gitverse ник)</div>
          <div></div>
          <input className={`${inputClass}`} type="text" name="username" />
          <div>Пароль</div>
          <div></div>
          <input className={`${inputClass}`} type="password" name="password" />
          <div> </div>
          <button type="submit" className={buttonClass}>
            Отправить
          </button>
          <div> </div>
        </div>
      </Form>
    </div>
  );
};

export default Login;
