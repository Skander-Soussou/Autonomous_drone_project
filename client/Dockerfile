FROM node:10
RUN apt-get update && apt-get install

WORKDIR /app-client

COPY ./src ./src
COPY ./public ./public
COPY package.json yarn.lock ./

RUN yarn install
RUN yarn start

FROM nginx:1.12-alpine
COPY --from=build-deps /usr/src/app/build /usr/share/nginx/html
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]