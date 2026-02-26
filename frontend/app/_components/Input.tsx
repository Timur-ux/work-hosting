import { ValidatorType, ErrorSetter, EventType } from "@/_logic/validators";
import React, { ChangeEventHandler, useState } from "react";

export type ErrorString = string;

export interface InputProps {
  inputClass: string;
  type: string;
  name: string;
  validState: ErrorSetter;
  validator: (setter: ErrorSetter) => (e: EventType) => void;
}

const Input = ({
  inputClass,
  type,
  name,
  validState,
  validator,
}: InputProps) => {
  const errorClass = "border-red-500";
  const [error, setError] = validState;
  const validatorFunc = validator([error, setError]);
  const validateField: ChangeEventHandler<
    HTMLInputElement,
    HTMLInputElement
  > = (e) => validatorFunc(e);
  return (
    <div>
      <input
        className={`${inputClass} text-black border-solid border-4 ${error != null && errorClass}`}
        type={type}
        name={name}
        onChange={validateField}
      />
      <p className={` text-sm ${error == null ? "text-black" : "text-red-500"}`}> {error != null ? error : "label"}</p>
    </div>
  );
};

export default Input;
