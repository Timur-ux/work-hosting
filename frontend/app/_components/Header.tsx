import Link from "next/link"

export default function Header() {
	const buttonClass="rounded-xl bg-gray-500 p-3"
	return (
		<div className="flex flex-col justify-center min-h-30 border-solid border-2 border-white">
		<div className="flex justify-center text-2xl">
		Сайт предварительной проверки работ
		</div>
		<div className="flex justify-between px-10 py-5">
		<Link href="/list" className={buttonClass}>
		Очередь проверки
		</Link>
		<Link href="/send" className={buttonClass}>
		Отослать работу
		</Link>
		<Link href="/check" className={buttonClass}>
		Посмотреть результат проверки
		</Link>
		</div>
		</div>
	)
}
