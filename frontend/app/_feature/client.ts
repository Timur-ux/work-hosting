import axios from "axios";

const client = axios.create({
  baseURL: "http://localhost:80/api",
});

// client.interceptors.request.use(
//   function (config) {
//     const bearer_token = localStorage.getItem("bearer-token");
//     if (bearer_token != null)
//       config.headers.setAuthorization(`Bearer ${bearer_token}`);
//
//     return config;
//   },
//   function (error) {
//     console.error("Axios request error: ", error);
//     return Promise.reject(error);
//   },
// );

export type RequestError = {
  status: number;
  message: string;
};

export default client;
