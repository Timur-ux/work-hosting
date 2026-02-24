"use client";

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
			<div>
			<div className="w-full underline flex justify-center mb-20 text-xl">
			Отправка работ
			</div>
			<div>
				{children}
			</div>
			</div>
  );
}

