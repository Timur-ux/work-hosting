"use client";
import React, { useState } from "react";
import Form from "next/form";
import { buttonClass } from "@/_styles/globals";
import { IsValidResponse, RequestError } from "@/_feature/client";
import { useAppDispatch } from "@/hooks";
import GetUserData from "@/_feature/userData";
import DoRegister from "@/_feature/register";
import Input, { ErrorString } from "@/_components/Input";
import Link from "next/link";
import {
  CastValidator,
  Combine,
  GitVerseNameValidator,
  MaxLengthValidator,
  MinLengthValidator,
} from "@/_logic/validators";

const RegisterError = ({ error }: { error: RequestError }) => {
	console.log("REGISTER ERROR:", error);
  const message =
    error.status == 404 ? "Студент с такими данными не найден" : error.message;
  return (
    <div className="w-full text-red-500 flex justify-center">{`Error: ${message}`}</div>
  );
};

const Register = () => {
  const [error, setError] = useState<RequestError | null>(null);
  const [registerStatus, setRegisterStatus] = useState<boolean>(false);
  const dispatch = useAppDispatch();
  const firstNameError = useState<ErrorString | null>(null);
  const lastNameError = useState<ErrorString | null>(null);
  const fatherNameError = useState<ErrorString | null>(null);
  const gvNameError = useState<ErrorString | null>(null);
  const emailError = useState<ErrorString | null>(null);
  const passwordError = useState<ErrorString | null>(null);

  async function ProcessRegister(formData: FormData) {
    setError(null);
    setRegisterStatus(false);
    const first_name = formData.get("first-name") as string;
    const last_name = formData.get("last-name") as string;
    const father_name = formData.get("father-name") as string;
    const email = formData.get("email") as string;
    const username = formData.get("username") as string;
    const password = formData.get("password") as string;

    const fields = [
      first_name,
      last_name,
      father_name,
      email,
      username,
      password,
    ];
    if (
      !fields.reduce(
        (acc: boolean, field, _, __) => acc && field.length != 0,
        true,
      )
    ) {
      setError({
        status: 403,
        message: "Все поля ввода должны быть непустыми",
      });
      return;
    } else if (
      !fields.reduce((acc: boolean, field, _, __) => acc && field != null, true)
    ) {
      setError({ status: 403, message: "Неправильный формат текстовых полей" });
      return;
    }
    const response = await DoRegister(
      username,
      password,
      first_name,
      last_name,
      father_name,
      email,
    );
    if (!IsValidResponse(response)) {
			console.log("REGISTER RESPONSE ERROR: ", response);
      setError({
        status: response.error?.status as number,
        message: response.error?.message as string,
      });
      return;
    }
    setRegisterStatus(true);
    return;
  }

  const inputClass = "bg-white rounded w-full md:px-5 md:py-2 text-black";
  return (
    <div className="text-sm md:text-xl">
      {error !== null && <RegisterError error={error} />}
      {registerStatus && (
        <div className="text-green-500 text-xl w-full flex justify-center">
          Профиль успешно зарегистрирован, теперь вы можете войти в него!
        </div>
      )}
      <div className="w-full flex justify-center text-sm my-5 text-center font-bold">
        При регистрации важно, чтобы фамилия и имя совпадали фамилией и именем,
        указанным в таблице оценок
        <br />
        Почта не проверяется при регистрации, она нужна только для
        восстановления пароля
      </div>
      <Form className="items-center" action={ProcessRegister}>
        <div className="grid grid-cols-3 grid-rows-6 place-items-center items-baseline">
          <div>Фамилия</div>
          <div></div>
          <Input
            inputClass={inputClass}
            type="text"
            name="last-name"
            validState={lastNameError}
            validator={CastValidator(
              Combine([MinLengthValidator(2), MaxLengthValidator(30)]),
            )}
          />
          <div>Имя</div>
          <div></div>
          <Input
            inputClass={inputClass}
            type="text"
            name="first-name"
            validState={firstNameError}
            validator={CastValidator(
              Combine([MinLengthValidator(2), MaxLengthValidator(30)]),
            )}
          />
          <div>Отчество</div>
          <div></div>
          <Input
            inputClass={inputClass}
            type="text"
            name="father-name"
            validState={fatherNameError}
            validator={CastValidator(
              Combine([MinLengthValidator(2), MaxLengthValidator(30)]),
            )}
          />
          <div>Почта</div>
          <div></div>
          <Input
            inputClass={inputClass}
            type="text"
            name="email"
            validState={emailError}
            validator={CastValidator(
              Combine([MinLengthValidator(2), MaxLengthValidator(128)]),
            )}
          />
          <div>Логин(gitverse ник)</div>
          <div></div>
          <Input
            inputClass={inputClass}
            type="text"
            name="username"
            validState={gvNameError}
            validator={CastValidator(
              Combine([
                GitVerseNameValidator(),
                MinLengthValidator(3),
                MaxLengthValidator(60),
              ]),
            )}
          />
          <div>Пароль</div>
          <div></div>
          <Input
            inputClass={inputClass}
            type="password"
            name="password"
            validState={passwordError}
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
        <Link href="/login">Есть профиль? Войди в него!</Link>
      </div>
    </div>
  );
};

export default Register;
