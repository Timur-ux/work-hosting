"use client";
import React, { useState } from "react";
import UserData from "../_components/UserData";

const Send = () => {
  return (
    <div>
      <UserData />
      <div className="flex w-full justify-center">
        <button className="bg-blue-500 rounded-xl p-3">Отправить работу</button>
      </div>
    </div>
  );
};

export default Send;
