# A single spawned process example.
# Invoke by running `iex ./actors_elixir.exs`

myFirstProcess = spawn(fn ->
  IO.puts "I'm a processing waiting around for a message."
  receive do
    msg -> IO.puts "Got a message! It says: #{inspect msg}"
  end
end)

send(myFirstProcess, "Hello Process!")
