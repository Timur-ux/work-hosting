import { ChangeEvent, InputEvent } from "react";
import { ErrorString } from "@/_components/Input";

export type ErrorSetter = [
  ErrorString | null,
  React.Dispatch<React.SetStateAction<ErrorString | null>>,
];

export type EventType = ChangeEvent<HTMLInputElement>;

export type ValidatorType = (e: EventType) => ErrorString | null;

export const CastValidator =
  (validator: ValidatorType) =>
  ([_, setError]: ErrorSetter) =>
  (e: EventType) => {
    const result = validator(e);
    setError(result);
    return;
  };

export const MinLengthValidator: (minLen: number) => ValidatorType = (minLen) => {
  return (e) => {
    if (e.target.value.length == 0) {
      return null;
    }
    if (e.target.value.length < minLen) {
      return `Минимальная длина поля: ${minLen}`;
    }
    return null;
  };
};
export const MaxLengthValidator: (maxLen: number) => ValidatorType = (maxLen) => {
  return (e) => {
    if (e.target.value.length == 0) {
      return null;
    }
    if (e.target.value.length > maxLen) {
      return `Максимальная длина поля: ${maxLen}`;
    }
    return null;
  };
};

export const GitVerseNameValidator: () => ValidatorType = () => (e) => {
  if (e.target.value.length > 0 && e.target.value[0] == "@")
    return `Gitverse ник должен быть без @ в начале`;
  return null;
};

export const Combine: (
  validators: ValidatorType[],
) => ValidatorType = (validators) => {
  return (e) => {
    for (let i = 0; i < validators.length; ++i) {
      const result = validators[i](e);
			if(result != null)
				return result;
    }
		return null;
  };
};
