"use client";
import { SelectIsLoggedIn, SelectProfile } from "@/_reducers/profile";
import { buttonClass } from "@/_styles/globals";
import { useAppSelector } from "@/hooks";

const Profile = () => {
  const profile = useAppSelector(SelectProfile);
  const loggedIn = useAppSelector(SelectIsLoggedIn);
  if (profile == null || !loggedIn) {
    return <div>Вы не вошли в профиль, нечего здесь смотреть</div>;
  }

  if (profile.role == "admin") {
    return (
      <div className="text-xl grid gap-10 grid-cols-2 grid-rows-1">
        <div>Имя пользователя:</div>
        <div>{profile.username}</div>
      </div>
    );
  }

  const changeFields = async () => {
    alert("Пока не реализовано");
  };

  return (
    <div className="my-10">
      <div className="text-xl grid gap-10 grid-cols-2 justify-between">
        <div>Имя пользователя:</div>
        <div className="flex justify-end">{profile.username}</div>
        <div>Фамилия:</div>
        <div className="flex justify-end">{profile.data?.last_name}</div>
        <div>Имя:</div>
        <div className="flex justify-end">{profile.data?.first_name}</div>
        <div>Отчество:</div>
        <div className="flex justify-end">{profile.data?.father_name}</div>
        <div>Номер группы:</div>
        <div className="flex justify-end">{profile.data?.group_number}</div>
        <div>Номер по списку в группе:</div>
        <div className="flex justify-end">{profile.data?.in_group_order}</div>
        <div>Почта</div>
        <div className="flex justify-end">{profile.data?.email}</div>
      </div>
			<div className="w-full flex justify-center mt-5">
			<button onClick={changeFields} className={`${buttonClass}`}>
				Изменить данные профиля
			</button>
			</div>
    </div>
  );
};

export default Profile;
