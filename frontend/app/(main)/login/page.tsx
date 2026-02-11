"use client";
import React, { useState } from "react";
import Form from "next/form";
import { buttonClass } from "@/_styles/globals";
import LoginAndGetToken from "@/_feature/login";
import { IsValidResponse, RequestError } from "@/_feature/client";
import { useAppDispatch } from "@/hooks";
import GetUserData from "@/_feature/userData";
import { login, logout, ProfileData } from "@/_reducers/profile";
import Input, { ErrorString } from "@/_components/Input";
import Link from "next/link";
import { CastValidator, Combine, GitVerseNameValidator, MaxLengthValidator, MinLengthValidator } from "@/_logic/validators";

const LoginError = ({ error }: { error: RequestError }) => {
  console.log("LOGIN ERROR:", error);
  const message =
    error.status == 404 ? "Неправильный логин или пароль" : error.message;
  return (
    <div className="w-full text-red-500 flex justify-center">{`Error: ${message}`}</div>
  );
};

const Login = () => {
  const [error, setError] = useState<RequestError | null>(null);
  const [loginStatus, setLoginStatus] = useState<boolean>(false);
  const dispatch = useAppDispatch();
  const [loginError, setLoginError] = useState<ErrorString | null>(null);
  const [passwordError, setPasswordError] = useState<ErrorString | null>(null);

  async function ProcessLogin(formData: FormData) {
    setError(null);
    setLoginStatus(false);
    dispatch(logout({}));
    const username = formData.get("username") as string;
    const password = formData.get("password") as string;
    if (username.length == 0 || password.length == 0) {
      setError({
        status: 403,
        message: "Все поля ввода должны быть непустыми",
      });
      return;
    } else if (loginError != null || passwordError != null) {
      setError({ status: 403, message: "Неправильный формат текстовых полей" });
      return;
    }
    const response1 = await LoginAndGetToken(username, password);
    if (!IsValidResponse(response1)) {
      setError({
        status: response1.error?.status as number,
        message: response1.error?.message as string,
      });
      return;
    }
    const bearerToken = response1.payload as string;
    localStorage.setItem("bearer-token", bearerToken);

    const response2 = await GetUserData(bearerToken, username);
    if (!IsValidResponse(response2)) {
      setError({
        status: response2.error?.status as number,
        message: response2.error?.message as string,
      });
      return;
    }
    dispatch(login({ profile: response2.payload as ProfileData }));
    setLoginStatus(true);
  }

  const inputClass = "bg-white rounded w-full md:px-5 md:py-2 text-black";
  return (
    <div className="text-sm md:text-xl">
      {error !== null && <LoginError error={error} />}
      {loginStatus && (
        <div className="text-green-500 text-xl w-full flex justify-center">
          Вы успешно вошли в профиль!
        </div>
      )}
      <Form className="items-center" action={ProcessLogin}>
        <div className="grid grid-cols-3 grid-rows-3 place-items-center items-baseline">
          <div>Логин(gitverse ник)</div>
          <div></div>
          <Input
            inputClass={inputClass}
            type="text"
            name="username"
            validState={[loginError, setLoginError]}
            validator={CastValidator(Combine([GitVerseNameValidator(), MinLengthValidator(3), MaxLengthValidator(60)]))}
          />
          <div>Пароль</div>
          <div></div>
          <Input
            inputClass={inputClass}
            type="password"
            name="password"
            validState={[passwordError, setPasswordError]}
						validator={CastValidator(MinLengthValidator(5))}
          />
          <div> </div>
          <button type="submit" className={buttonClass}>
            Отправить
          </button>
          <div> </div>
        </div>
      </Form>
      <div className="w-full flex justify-end text-blue-800">
        <Link href="/register">Нет профиля? Зарегистрируйся!</Link>
      </div>
    </div>
  );
};

export default Login;
