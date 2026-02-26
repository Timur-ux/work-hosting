"use client";
import { useAppSelector } from "@/hooks";
import { SelectProfile } from "@/_reducers/profile";

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  const profile = useAppSelector(SelectProfile);

  if (profile?.role != "admin")
    return <div>Эта страница только для администраторов, идите отсюда</div>;
  return children;
}
