"use client";
import Header from "@/_components/Header";
import Footer from "@/_components/Footer";
import { Provider } from "react-redux";
import { makeStore, AppStore } from "@/store";
import { useEffect, useRef } from "react";
import { useRouter } from "next/router";
import { useAppDispatch, useAppSelector } from "@/hooks";
import { login, ProfileData, SelectIsLoggedIn, SelectProfile } from "@/_reducers/profile";
import React from "react";
import GetUserData from "@/_feature/userData";
import { IsValidResponse } from "@/_feature/client";

const AuthChecker = () => {
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

	return ""
}

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  const storeRef = useRef<AppStore | null>(null);
  if (!storeRef.current) {
    storeRef.current = makeStore();
  }
  return (
    <Provider store={storeRef.current}>
			<AuthChecker/>
      <div className="my-10 flex flex-col w-full bg-black text-white h-screen">
        <Header />
        <div className="mb-auto flex w-full justify-center place-items-center h-full">
          {children}
        </div>
        <Footer />
      </div>
    </Provider>
  );
}
