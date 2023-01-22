<template>
  <nav class="bg-white px-2 sm:px-4 py-2.5 dark:bg-gray-900">
    <div class="container flex flex-wrap items-center justify-between mx-auto">
      <a href="#" class="flex items-center">
        <gauge-icon class="dark:text-white h-6 mr-3 sm:h-9" />
        <span
          class="self-center text-xl font-semibold whitespace-nowrap dark:text-white"
          >LogME</span
        >
      </a>
      <button
        data-collapse-toggle="navbar-default"
        type="button"
        class="inline-flex items-center p-2 ml-3 text-sm text-gray-500 rounded-lg md:hidden hover:bg-gray-100 focus:outline-none focus:ring-2 focus:ring-gray-200 dark:text-gray-400 dark:hover:bg-gray-700 dark:focus:ring-gray-600"
        aria-controls="navbar-default"
        aria-expanded="false"
      >
        <span class="sr-only">Open main menu</span>
        <svg
          class="w-6 h-6"
          aria-hidden="true"
          fill="currentColor"
          viewBox="0 0 20 20"
          xmlns="http://www.w3.org/2000/svg"
        >
          <path
            fill-rule="evenodd"
            d="M3 5a1 1 0 011-1h12a1 1 0 110 2H4a1 1 0 01-1-1zM3 10a1 1 0 011-1h12a1 1 0 110 2H4a1 1 0 01-1-1zM3 15a1 1 0 011-1h12a1 1 0 110 2H4a1 1 0 01-1-1z"
            clip-rule="evenodd"
          ></path>
        </svg>
      </button>
      <div class="hidden w-full md:block md:w-auto" id="navbar-default">
        <ul
          class="flex flex-col p-4 mt-4 rounded-lg bg-gray-50 md:flex-row md:space-x-8 md:mt-0 md:text-sm md:font-medium md:border-0 md:bg-white dark:bg-gray-800 md:dark:bg-gray-900 dark:border-gray-700"
        >
          <li class="flex items-center justify-center">
            <router-link class="dark:text-white" to="/">Home</router-link>
          </li>
          <li class="flex items-center justify-center">
            <router-link class="dark:text-white" to="/sensors"
              >Sensors</router-link
            >
          </li>
          <li class="flex items-center justify-center">
            <router-link class="dark:text-white" to="/endpoints"
              >Endpoints</router-link
            >
          </li>
          <li class="flex items-center justify-center">
            <router-link class="dark:text-white" to="/settings"
              >Settings</router-link
            >
          </li>
          <li>
            <DarkModeButton />
          </li>
        </ul>
      </div>
    </div>
  </nav>
</template>

<style scoped>
.router-link-exact-active {
  @apply text-blue-700;
}
</style>
<script setup lang="ts">
import GaugeIcon from '../icons/GaugeIcon.vue';
import DarkModeButton from '../buttons/DarkModeButton.vue';
</script>
