<template>
  <footer
    class="mt-5 p-4 bg-white shadow md:flex md:items-center md:justify-between md:p-6 dark:bg-gray-800"
  >
    <span class="text-sm text-gray-500 sm:text-center dark:text-gray-400"
      >© 2023 <a href="#" class="hover:underline">LogME</a>. All Rights
      Reserved.
    </span>
    <ul class="dark:text-gray-400 sm:mt-0">
      <li>
        <a
          href="https://github.com/DerZwergGimli/logme_interface"
          class="hover:underline"
          ><github-icon
        /></a>
      </li>
    </ul>
  </footer>
</template>
<script setup lang="ts">
import GithubIcon from '../icons/GithubIcon.vue';
</script>
