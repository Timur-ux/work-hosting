"use client";
import Header from "@/_components/Header";
import Footer from "@/_components/Footer";
import { Provider } from "react-redux";
import { makeStore, AppStore } from "@/store";
import { useRef } from "react";

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
      <div className="flex flex-col w-full bg-black text-white h-screen">
        <Header />
        <div className="mb-auto flex w-full justify-center place-items-center">
          {children}
        </div>
        <Footer />
      </div>
    </Provider>
  );
}
