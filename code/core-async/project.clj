(defproject core-async-ex "0.1.0-SNAPSHOT"
  :dependencies [[org.clojure/clojure "1.8.0"]
                 [org.clojure/core.async "0.4.474"]]
  :main ^:skip-aot core-async-ex.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all}})
