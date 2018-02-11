(ns the-pipe.core
  (:require [clojure.core.async
             :as async
             :refer [>! <! >!! <!! go chan]]))


;; TODO
(defn my-concurrent-func []
  ;; First let's create a channel
  ;; This channel is responsible for communicating messages with different processes.
  (let [my-channel (chan)]

      ;; Each "go block" is going to happen on a seperate thread.

    ))

