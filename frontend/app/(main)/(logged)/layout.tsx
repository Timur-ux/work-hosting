"use client";
import { useAppSelector } from "@/hooks";
import { SelectProfile } from "@/_reducers/profile";
import { useRouter } from "next/navigation";
import { useEffect } from "react";

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  const profile = useAppSelector(SelectProfile);
  const router = useRouter();

  useEffect(() => {
    if (typeof window == "undefined" && profile == null) router.push("/login");
  }, []);

  return children;
}
