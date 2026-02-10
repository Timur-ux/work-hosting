import { RequestError } from "@/_feature/client";
import React from "react";

const NetworkError = ({error}: {error: RequestError}) => {
	return <div className="w-full text-red-500 flex justify-center">{`Error: ${error.message}. Error code: ${error.status}`}</div>
}

export default NetworkError;
