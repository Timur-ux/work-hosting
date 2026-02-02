export default function Footer() {
  const hrefClass = "bg-gray-500 rounded p-1 mx-3";
  return (
    <footer className="flex justify-between px-20">
      <div className="flex">
        Весь сервис описал Бучкин Тимур
        <div className="flex">
          <a href="https://github.com/Timur-ux" className={hrefClass}>
            github
          </a>{" "}
          <a href="https://gitverse.ru/Tias" className={hrefClass}>
            gitverse
          </a>
        </div>
      </div>
      <div>
        Репозиторий сервиса:{" "}
        <a
          href="https://github.com/Timur-ux/work-hosting"
          className={hrefClass}
        >
          github
        </a>
      </div>
    </footer>
  );
}
