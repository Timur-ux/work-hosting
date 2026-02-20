import { RequestError } from "@/_feature/client";
import React from "react";

const NetworkError = ({error}: {error: RequestError}) => {
	console.log("NetError: ", error)
	return <div className="w-full text-red-500 flex justify-center">{`Error: ${error.message}`}</div>
}

export default NetworkError;
