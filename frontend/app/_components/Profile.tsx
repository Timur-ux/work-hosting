import { SelectIsLoggedIn, SelectProfile } from "@/_reducers/profile";
import { useAppSelector } from "@/hooks";
import { Button, Menu, MenuItem } from "@mui/material";
import Link from "next/link";
import React, { useState } from "react";
import { buttonClass } from "@/_styles/globals";

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
  const loggedIn = useAppSelector(SelectIsLoggedIn);
  const profile = useAppSelector(SelectProfile);
  const [anchorElement, setAnchorElement] = React.useState<null | HTMLElement>(
    null,
  );

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
    alert(`Redirecting to ${href}`);
  };

  const menuOpts: MenuOpt[] = [];
  menuOpts.push({ href: "/profile", text: "Профиль" });
  if (profile.role == "student") {
    menuOpts.push({ href: "/student/marks", text: "Оценки" });
    menuOpts.push({ href: "/student/send", text: "Отправить работу" });
    menuOpts.push({ href: "/student/logs", text: "Отчет о проверке" });
  } else if (profile.role == "admin") {
    menuOpts.push({ href: "/admin/dashboard", text: "Мониторинг" });
  }
  menuOpts.push({ href: "/logout", text: "Выйти из профиля" });

	async function checkToken() {

	}

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
        Профиль
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
        {menuOpts.map((value) => (
          <MenuItem
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
