"use client";
import { login, logout, ProfileData, SelectIsLoggedIn, SelectProfile } from "@/_reducers/profile";
import { useAppDispatch, useAppSelector } from "@/hooks";
import { Button, Menu, MenuItem } from "@mui/material";
import Link from "next/link";
import React, { useEffect } from "react";
import { buttonClass } from "@/_styles/globals";
import GetUserData from "@/_feature/userData";
import { IsValidResponse } from "@/_feature/client";
import { useRouter } from "next/navigation";

type Props = {
  buttonClass?: string;
  menuOptClass?: string;
};

const defaultProps: Props = {
  buttonClass: buttonClass,
  menuOptClass: "bg-gray-900 p-2 rounded hover:bg-gray-500",
};

type MenuOpt = {
  href: string;
  text: string;
};

const Profile = ({
  buttonClass = defaultProps.buttonClass,
  menuOptClass = defaultProps.menuOptClass,
}: Props) => {
	const router = useRouter()
  const loggedIn = useAppSelector(SelectIsLoggedIn);
  const profile = useAppSelector(SelectProfile);
	const dispatch = useAppDispatch();
  const [anchorElement, setAnchorElement] = React.useState<null | HTMLElement>(
    null,
  );
  async function checkToken() {
    if (typeof localStorage == "undefined") 
			return;
		const token = localStorage.getItem("bearer-token");
		const username = localStorage.getItem("username")
		if (token == null) {
			console.log('auth token not found in local storage');
			return;
		}
		if(username == null) {
			console.log('auth token found in local storage but username not set');
			return;
		}
		
		console.log("Found auth token in local storage, trying to auth");
		const response = await GetUserData(token, username);
    if (!IsValidResponse(response)) {
			if(response.error?.status == 401) 
				console.log("token expired, remove it from local storage")
			 else 
				console.log("Undefined error while trying fetch user data by token stored in local storage, removing it");
			
				localStorage.removeItem("bearer-token");
				localStorage.removeItem("username");
      return;
    }
    dispatch(login({ profile: response.payload as ProfileData }));
  }

  useEffect(() => {
    checkToken();
  }, []);

  if (!loggedIn) {
    return (
      <Link href="/login" className={buttonClass}>
        Войти в профиль
      </Link>
    );
  }
  if (profile === null) {
    console.error("Profile set as logged in but profile is null!");
    return (
      <Link href="/login" className={buttonClass}>
        Войти в профиль
      </Link>
    );
  }

  const isDroppedDown = Boolean(anchorElement);

  const onProfileClicked = (event: React.MouseEvent<HTMLButtonElement>) => {
    setAnchorElement(event.currentTarget);
  };

  const onMenuClose = () => {
    setAnchorElement(null);
  };

  const redirectTo = (href: string) => {
		if(href != "/logout") {
			router.push(href);
			return;
		}
		dispatch(logout({}));
  };

  const menuOpts: MenuOpt[] = [];
  menuOpts.push({ href: "/profile", text: "Профиль" });
  if (profile.role == "student") {
    menuOpts.push({ href: "/student/marks", text: "Оценки" });
    menuOpts.push({ href: "/student/send", text: "Отправить работу" });
    menuOpts.push({ href: "/student/logs", text: "Отчет о проверке" });
  } else if (profile.role == "admin") {
    menuOpts.push({ href: "/admin/work/add", text: "Добавить работу" });
    menuOpts.push({ href: "/admin/work/check", text: "Проверить работы" });
  }
  menuOpts.push({ href: "/logout", text: "Выйти из профиля" });

  return (
    <div>
      <Button
        id="profile-button"
        className={buttonClass}
        aria-controls="profile-menu"
        aria-haspopup="true"
        aria-expanded={isDroppedDown ? "true" : undefined}
        onClick={onProfileClicked}
      >
			{profile.username}
      </Button>
      <Menu
        id="profile-menu"
        anchorEl={anchorElement}
        open={isDroppedDown}
        onClose={onMenuClose}
        slotProps={{
          list: {
            "aria-labelledby": "profile-button",
          },
        }}
      >
        {menuOpts.map((value, index) => (
          <MenuItem
            key={index}
            className={menuOptClass}
            onClick={() => redirectTo(value.href)}
          >
            {value.text}
          </MenuItem>
        ))}
      </Menu>
    </div>
  );
};

export default Profile;
