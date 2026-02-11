import Link from "next/link"
import Profile from "./Profile"
import { buttonClass } from "@/_styles/globals"

export default function Header() {
	return (
		<div className="flex flex-col justify-center min-h-30 border-b-solid border-b-2 border-b-white">
		<div className="flex justify-center text-2xl">
		Сайт предварительной проверки работ !!!!!!!!!!
		</div>
		<div className="flex justify-between px-10 py-5">
		<Link href="/" className={buttonClass}>
		Домашняя страница
		</Link>
		<Link href="/list" className={buttonClass}>
		Очередь проверки
		</Link>
		<Profile />
		</div>
		</div>
	)
}
